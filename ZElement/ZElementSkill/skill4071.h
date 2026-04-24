#ifndef __CPPGEN_GNET_SKILL4071
#define __CPPGEN_GNET_SKILL4071
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4071:public Skill
    {
      public:
        enum
        { SKILL_ID = 4071 };
          Skill4071 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4071Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1600;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar11 (skill->GetPlayer ()->GetOccupation ());
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetMaxhp ());
                skill->GetPlayer ()->SetVar3 (skill->GetPlayer ()->GetMaxmp ());
                skill->GetPlayer ()->SetVar4 (skill->GetPlayer ()->GetHp ());
                skill->GetPlayer ()->SetVar5 (skill->GetPlayer ()->GetMp ());
                skill->GetPlayer ()->SetVar6 (skill->GetPlayer ()->GetRes1 ());
                skill->GetPlayer ()->SetVar7 (skill->GetPlayer ()->GetRes2 ());
                skill->GetPlayer ()->SetVar8 (skill->GetPlayer ()->GetRes3 ());
                skill->GetPlayer ()->SetVar9 (skill->GetPlayer ()->GetRes4 ());
                skill->GetPlayer ()->SetVar10 (skill->GetPlayer ()->GetRes5 ());
                skill->GetPlayer ()->SetVar16 (zrand (100) < (skill->GetLevel () + 1) * (skill->GetLevel () + 1) ? 1 : 2);
                skill->SetRatio (1 + 0.1 * skill->GetT0 () + 0.07 * skill->GetLevel () +
                                 0.01 * INT (skill->GetPlayer ()->GetLevel () / (9 - skill->GetLevel ())));
                skill->SetPlus (215 * skill->GetLevel () + 0.03 * skill->GetT1 () * skill->GetPlayer ()->GetDef ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4071Stub ():SkillStub (4071)
        {
            occupation = 130;
            name = L"真·暗香无际";
            nativename = "真·暗香无际";
            icon = "飞升暗香无际.dds";
            maxlevel = 5;
            maxlearn = 5;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = -2;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 0;
            talent[0] = 721;
            talent[1] = 1582;
            talent_size = 2;
            action[0] = "鬼王宗_法宝_莲花";
            action[1] = "青云门_法宝_莲花";
            action[2] = "鬼王宗_法宝_莲花";
            action[3] = "合欢派_法宝_莲花";
            action[4] = "天音寺_法宝_莲花";
            action[5] = "鬼道_法宝_莲花";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "鬼王宗_骑乘_法宝_莲花";
            action[16] = "青云门_骑乘_法宝_莲花";
            action[17] = "鬼王宗_骑乘_法宝_莲花";
            action[18] = "合欢派_骑乘_法宝_莲花";
            action[19] = "天音寺_骑乘_法宝_莲花";
            action[20] = "鬼道_法宝_莲花";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
            doenchant = true;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill4071Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05 + 10);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (445 - 35 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6300 - 300 * skill->GetLevel ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05 + 5);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               7 * skill->GetLevel (),
                               225 * skill->GetLevel (),
                               9 - skill->GetLevel (),
                               445 - 35 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel (), 6.3 - 0.3 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetTalismaneffects (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTalismaneffectsbless (1);
            return true;
        }
#endif
    };
}
#endif
