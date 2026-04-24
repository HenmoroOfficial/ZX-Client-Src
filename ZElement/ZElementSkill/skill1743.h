#ifndef __CPPGEN_GNET_SKILL1743
#define __CPPGEN_GNET_SKILL1743
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1743:public Skill
    {
      public:
        enum
        { SKILL_ID = 1743 };
          Skill1743 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1743Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1000;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 10000;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1743Stub ():SkillStub (1743)
        {
            occupation = 151;
            name = L"ÐþÌìÔÆÎè";
            nativename = "ÐþÌìÔÆÎè";
            icon = "ÐþÌìÔÆÎè.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 1;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
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
            action[0] = "Â½Ñ©ç÷Ê±×°Ðã";
            action[1] = "Â½Ñ©ç÷Ê±×°Ðã";
            action[2] = "Â½Ñ©ç÷Ê±×°Ðã";
            action[3] = "Â½Ñ©ç÷Ê±×°Ðã";
            action[4] = "Â½Ñ©ç÷Ê±×°Ðã";
            action[5] = "Â½Ñ©ç÷Ê±×°Ðã";
            action[6] = "Â½Ñ©ç÷Ê±×°Ðã";
            action[7] = "Â½Ñ©ç÷Ê±×°Ðã";
            action[8] = "Â½Ñ©ç÷Ê±×°Ðã";
            action[9] = "Â½Ñ©ç÷Ê±×°Ðã";
            action[10] = "Â½Ñ©ç÷Ê±×°Ðã";
            action[11] = "Â½Ñ©ç÷Ê±×°Ðã";
            action[12] = "Â½Ñ©ç÷Ê±×°Ðã";
            action[13] = "";
            action[14] = "";
            action[15] = "0";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 5;
            doenchant = false;
            dobless = false;
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
        virtual ~ Skill1743Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (205);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 11000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 15000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 205);

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
#endif
    };
}
#endif
